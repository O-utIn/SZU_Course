package com.example.mynews;

import android.content.Intent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import androidx.annotation.NonNull;
import androidx.recyclerview.widget.RecyclerView;
import java.util.List;

public class ItemAdapter extends RecyclerView.Adapter<ItemAdapter.ViewHolder> {
    private List<PreferenceItem> mItemList;

    static class ViewHolder extends RecyclerView.ViewHolder {
        TextView tvTitle, tvSource, tvTime;
        View itemView;

        public ViewHolder(View view) {
            super(view);
            itemView = view;
            tvTitle = view.findViewById(R.id.tv_title);
            tvSource = view.findViewById(R.id.tv_source);
            tvTime = view.findViewById(R.id.tv_time);
        }
    }

    public ItemAdapter(List<PreferenceItem> itemList) {
        mItemList = itemList;
    }

    @NonNull
    @Override
    public ViewHolder onCreateViewHolder(@NonNull ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.item_news, parent, false);
        final ViewHolder holder = new ViewHolder(view);

        // 活动跳转。点击列表项跳转到详情页
        holder.itemView.setOnClickListener(v -> {
            int position = holder.getAdapterPosition();
            PreferenceItem item = mItemList.get(position);
            Intent intent = new Intent(parent.getContext(), DetailActivity.class);
            intent.putExtra("title", item.getTitle());
            intent.putExtra("url", item.getUrl());
            parent.getContext().startActivity(intent);
        });
        return holder;
    }

    @Override
    public void onBindViewHolder(@NonNull ViewHolder holder, int position) {
        PreferenceItem item = mItemList.get(position);
        holder.tvTitle.setText(item.getTitle());
        holder.tvSource.setText(item.getSource());
        holder.tvTime.setText(item.getTime());
    }

    @Override
    public int getItemCount() { return mItemList.size(); }
}